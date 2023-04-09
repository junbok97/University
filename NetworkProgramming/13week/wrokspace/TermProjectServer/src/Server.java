import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.UnsupportedEncodingException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextArea;
import javax.swing.JTextField;
import javax.swing.SwingConstants;
import javax.swing.border.EmptyBorder;

public class Server extends JFrame {

	private static final long serialVersionUID = 1L;
	private JPanel contentPane;
	JTextArea textArea;
	private JTextField txtPortNumber;

	private ServerSocket socket; // 서버소켓
	private Socket client_socket; // accept() 에서 생성된 client 소켓
	private Vector<UserService> UserServiceVec = new Vector<UserService>(); // 연결된 사용자를 저장할 벡터
	private static final int BUF_LEN = 128; // Windows 처럼 BUF_LEN 을 정의
	private static int room_no = 0;
	private Vector<Room> RoomVec = new Vector<Room>();

	class Room {
		public Vector<UserService> room_userlist;
		public int room_id;

		public Room(Vector<UserService> room_userlist, int room_id) {
			this.room_userlist = room_userlist;
			this.room_id = room_id;
		}
	}

	/**
	 * Launch the application.
	 */
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					Server frame = new Server();
					frame.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}

	/**
	 * Create the frame.
	 */
	public Server() {

		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setBounds(100, 100, 338, 440);
		contentPane = new JPanel();
		contentPane.setBorder(new EmptyBorder(5, 5, 5, 5));
		setContentPane(contentPane);
		contentPane.setLayout(null);

		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(12, 10, 300, 298);
		contentPane.add(scrollPane);

		textArea = new JTextArea();
		textArea.setEditable(false);
		scrollPane.setViewportView(textArea);

		JLabel lblNewLabel = new JLabel("Port Number");
		lblNewLabel.setBounds(13, 318, 87, 26);
		contentPane.add(lblNewLabel);

		txtPortNumber = new JTextField();
		txtPortNumber.setHorizontalAlignment(SwingConstants.CENTER);
		txtPortNumber.setText("40000");
		txtPortNumber.setBounds(112, 318, 199, 26);
		contentPane.add(txtPortNumber);
		txtPortNumber.setColumns(10);

		JButton btnServerStart = new JButton("Server Start");
		btnServerStart.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				try {
					socket = new ServerSocket(Integer.parseInt(txtPortNumber.getText()));
				} catch (NumberFormatException | IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				AppendText("Chat Server Running..");
				btnServerStart.setText("Chat Server Running..");
				btnServerStart.setEnabled(false); // 서버를 더이상 실행시키지 못 하게 막는다
				txtPortNumber.setEnabled(false); // 더이상 포트번호 수정못 하게 막는다
				AcceptServer accept_server = new AcceptServer();
				accept_server.start();
			}
		});
		btnServerStart.setBounds(12, 356, 300, 35);
		contentPane.add(btnServerStart);
	}

	// 새로운 참가자 accept() 하고 user thread를 새로 생성한다.
	class AcceptServer extends Thread {
		@SuppressWarnings("unchecked")
		public void run() {
			while (true) { // 사용자 접속을 계속해서 받기 위해 while문
				try {
					AppendText("Waiting new clients ...");
					client_socket = socket.accept(); // accept가 일어나기 전까지는 무한 대기중
					AppendText("새로운 참가자 from " + client_socket);
					// User 당 하나씩 Thread 생성
					UserService new_user = new UserService(client_socket);
					UserServiceVec.add(new_user); // 새로운 참가자 배열에 추가

					new_user.start(); // 만든 객체의 스레드 실행
					AppendText("현재 참가자 수 " + UserServiceVec.size());
				} catch (IOException e) {
					AppendText("accept() error");
					// System.exit(0);

				}
			}
		}
	}

	public void AppendText(String str) {
		// textArea.append("사용자로부터 들어온 메세지 : " + str+"\n");
		textArea.append(str + "\n");
		textArea.setCaretPosition(textArea.getText().length());
	}

	// User 당 생성되는 Thread
	// Read One 에서 대기 -> Write All
	class UserService extends Thread {

		private ObjectInputStream ois;
		private ObjectOutputStream oos;

		private Socket client_socket;
		private Vector<UserService> user_service_vc;
		public User user;

		public UserService(Socket client_socket) {
			// TODO Auto-generated constructor stub
			// 매개변수로 넘어온 자료 저장
			this.client_socket = client_socket;
			this.user_service_vc = UserServiceVec;
			try {

				oos = new ObjectOutputStream(client_socket.getOutputStream());
				oos.flush();
				ois = new ObjectInputStream(client_socket.getInputStream());

			} catch (Exception e) {
				AppendText("userService error");
			}
		}

		public void sendlist() {
			Vector<User> userlist = new Vector<User>();
			Iterator<UserService> it = user_service_vc.iterator();
			while (it.hasNext()) {
				User u = it.next().user;
				userlist.add(u);
			}
			WriteAllObject(new Userlist(userlist));
		}

		public void Login() {
			sendlist();
		}

		public void Logout() {
			UserServiceVec.removeElement(this);
			sendlist();
		}

		// 모든 User들에게 Object를 방송. 채팅 message와 image object를 보낼 수 있다
		public void WriteAllObject(Object ob) {
			Iterator<UserService> it = user_service_vc.iterator();
			while (it.hasNext()) {
				UserService user_service = it.next();
				user_service.WriteOneObject(ob);
			}
		}

		public void WriteOneObject(Object ob) {
			try {
				oos.writeObject(ob);
				oos.reset();
			} catch (IOException e) {
				AppendText("oos.writeObject(ob) error");
				try {
					ois.close();
					oos.close();
					client_socket.close();
					client_socket = null;
					ois = null;
					oos = null;
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				}
				Logout();
			}
		}

		public void WriteMsg(Msg msg) {
			try {
				for (int i = 0; i < RoomVec.size(); i++) {
					Room room = (Room) RoomVec.elementAt(i);
					if (room.room_id == msg.room_id) { // room_id를 찾고
						for (int j = 0; j < room.room_userlist.size(); j++) {
							UserService us = (UserService) room.room_userlist.elementAt(j);
							us.oos.writeObject(msg);
						}
					} // if (room.room_id == msg.room_id) 문 끝
				} // RoomVec for문 끝
			} catch (IOException e) {
				AppendText("dos.writeObject() error");
				try {
					oos.close();
					client_socket.close();
					client_socket = null;
					ois = null;
					oos = null;
				} catch (IOException e1) {
					// TODO Auto-generated catch block
					e1.printStackTrace();
				} // e1 catch문 종료
				Logout(); // 에러가난 현재 객체를 벡터에서 지운다
			} // e catch문 종료
		} // WriteMsg 함수 종료

		// Windows 처럼 message 제외한 나머지 부분은 NULL 로 만들기 위한 함수
		public byte[] MakePacket(String msg) {
			byte[] packet = new byte[BUF_LEN];
			byte[] bb = null;
			int i;
			for (i = 0; i < BUF_LEN; i++)
				packet[i] = 0;
			try {
				bb = msg.getBytes("euc-kr");
			} catch (UnsupportedEncodingException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
			for (i = 0; i < bb.length; i++)
				packet[i] = bb[i];
			return packet;
		}

		public void CreateRoom(Msg msg) { // room_id 생성
			msg.room_id = room_no;
			Vector<UserService> usv = new Vector<UserService>();

			for (int i = 0; i < UserServiceVec.size(); i++) {
				UserService us = (UserService) UserServiceVec.elementAt(i);
				User u1 = us.user;
				for (int j = 0; j < msg.UserVec.size(); j++) {
					User u2 = (User) msg.UserVec.elementAt(j);
					if (u1.name.equals(u2.name)) {
						usv.add(us);
						break;
					}
				}

			}

			RoomVec.add(new Room(usv, room_no++));
			WriteMsg(msg);
		}

		public void UpdateRoom(Msg msg) {
			/*
			 * Vector<User> uv = msg.UserVec;
			 * 
			 * for (int i = 0; i < RoomVec.size(); i++) { Room room = (Room)
			 * RoomVec.elementAt(i); if (room.room_id == msg.room_id) { for (int j = 0; j <
			 * uv.size(); j++) { User u = (User) uv.elementAt(i); room.room_userlist.add(u);
			 * Msg new_msg = new Msg(u, 100, room.room_id); WriteMsg(new_msg); } break; } }
			 */

		}

		public void run() {
			while (true) { // 사용자 접속을 계속해서 받기 위해 while문
				try {
					Object obcm = null;
					Msg msg = null;
					if (socket == null)
						break;
					try {
						obcm = ois.readObject();
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
						return;
					}
					if (obcm == null)
						break;
					if (obcm instanceof User) {
						this.user = (User) obcm;
						Login();
					}
					if (obcm instanceof Msg) {
						msg = (Msg) obcm;
						AppendText(msg.user.name);
						switch (msg.code) {
						case 100:
							CreateRoom(msg);
							break;
						case 101:
							UpdateRoom(msg);
							break;
						case 200:
							WriteMsg(msg);
						}
					} else
						continue;
					/*
					 * if (cm.getCode().matches("200")) { WritePrivate(cm);
					 * AppendText("Send_User : " + cm.send_user.name + " Recv_User : " +
					 * cm.recv_user.name); } if (cm.getCode().matches("300")) { WritePrivate(cm);
					 * AppendText("Send_User : " + cm.send_user.name + " Recv_User : " +
					 * cm.recv_user.name); }
					 */
				} catch (IOException e) {
					AppendText("ois.readObject() error");
					try {
//						dos.close();
//						dis.close();
						ois.close();
						oos.close();
						client_socket.close();
						Logout(); // 에러가난 현재 객체를 벡터에서 지운다
						break;
					} catch (Exception ee) {
						break;
					} // catch문 끝
				} // 바깥 catch문 끝
			} // while 끝
		} // run 끝
	} // Userservice 끝

}
