import java.awt.EventQueue;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.io.UnsupportedEncodingException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Set;
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

	private HashMap<Integer, Room> RoomHash = new HashMap<Integer, Room>();
	private HashMap<String, UserService> USHash = new HashMap<String, UserService>();

	// 룸 클래스 룸에 들어와 있는 유저
	class Room {
		public Vector<UserService> uslist;
		public Vector<User> ulist;

		public Room(Vector<UserService> uslist, Vector<User> ulist) {
			this.uslist = uslist;
			this.ulist = ulist;
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
		private Vector<Integer> us_room_vc = new Vector<Integer>();
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
			Msg msg = new Msg(1);
			msg.UserVec = userlist;
			WriteAllObject(msg);
		}

		public void Login() {
			sendlist();
		}

		public void Logout() {

			Set<Integer> keys = RoomHash.keySet();
			Iterator<Integer> it = keys.iterator();
			Msg msg = new Msg(this.user, 202);
			msg.setData(this.user.name);

			while (it.hasNext()) {
				int key = it.next();
				Room r = RoomHash.get(key);
				r.uslist.remove(this);

				for (int i = 0; i < r.ulist.size(); i++) {
					User u = (User)r.ulist.elementAt(i);
					if(u.name.equals(this.user.name)) {
						r.ulist.remove(i);
						break;
					}
				}				
				if (r.uslist.size() == 0) {
					RoomHash.remove(key);
					continue;
				}
				msg.room_id = key;
				WriteMsg(msg);
			}

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
				Room r = RoomHash.get(msg.room_id);
				for (int j = 0; j < r.uslist.size(); j++) {
					UserService us = (UserService) r.uslist.elementAt(j);
					us.oos.writeObject(msg);
					us.oos.reset();
				}

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
			this.us_room_vc.add(msg.room_id);

			Vector<UserService> usv = new Vector<UserService>();

			for (int i = 0; i < msg.UserVec.size(); i++) {
				User u = msg.UserVec.elementAt(i);
				UserService us = USHash.get(u.name);
				usv.add(us);
			}
			RoomHash.put(room_no++, new Room(usv, msg.UserVec));
			WriteMsg(msg);

			msg.code = 201;
			for (int i = 0; i < msg.UserVec.size(); i++) {
				User u = (User) msg.UserVec.elementAt(i);
				msg.setData(u.name);
				WriteMsg(msg);
			}

		}

		public void UpdateRoom(Msg msg) {

			Room r = RoomHash.get(msg.room_id);
			msg.code = 201;

			for (int i = 0; i < msg.UserVec.size(); i++) {
				User u = msg.UserVec.elementAt(i);
				msg.setData(u.name);
				WriteMsg(msg);
				r.ulist.add(u);
			}

			// 기존유저들에게 신규 유저 전송
			for (int i = 0; i < r.uslist.size(); i++) {
				UserService us = r.uslist.elementAt(i);
				Msg new_msg = new Msg(us.user, 101, msg.room_id);
				new_msg.UserVec = r.ulist;
				us.WriteOneObject(new_msg);
			}

			// 신규유저에게 채팅방 생성
			for (int i = 0; i < msg.UserVec.size(); i++) {
				User u = msg.UserVec.elementAt(i);
				UserService us = USHash.get(u.name);
				r.uslist.add(us);
				Msg new_msg = new Msg(u, 100, msg.room_id);
				new_msg.UserVec = r.ulist;
				us.WriteOneObject(new_msg);
			}

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
					if (obcm instanceof Msg) {
						msg = (Msg) obcm;
						switch (msg.code) {
						case 0:
							this.user = msg.user;
							USHash.put(msg.user.name, this);
							Login();
							break;
						case 100:
							CreateRoom(msg);
							break;
						case 101:
							UpdateRoom(msg);
							break;
						default:
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
