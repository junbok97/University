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

	private ServerSocket socket; // ��������
	private Socket client_socket; // accept() ���� ������ client ����
	private Vector<UserService> UserServiceVec = new Vector<UserService>(); // ����� ����ڸ� ������ ����
	private static final int BUF_LEN = 128; // Windows ó�� BUF_LEN �� ����
	private static int room_no = 0;

	private HashMap<Integer, Room> RoomHash = new HashMap<Integer, Room>();
	private HashMap<String, UserService> USHash = new HashMap<String, UserService>();

	// �� Ŭ���� �뿡 ���� �ִ� ����
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
				btnServerStart.setEnabled(false); // ������ ���̻� �����Ű�� �� �ϰ� ���´�
				txtPortNumber.setEnabled(false); // ���̻� ��Ʈ��ȣ ������ �ϰ� ���´�
				AcceptServer accept_server = new AcceptServer();
				accept_server.start();
			}
		});
		btnServerStart.setBounds(12, 356, 300, 35);
		contentPane.add(btnServerStart);
	}

	// ���ο� ������ accept() �ϰ� user thread�� ���� �����Ѵ�.
	class AcceptServer extends Thread {
		@SuppressWarnings("unchecked")
		public void run() {
			while (true) { // ����� ������ ����ؼ� �ޱ� ���� while��
				try {
					AppendText("Waiting new clients ...");
					client_socket = socket.accept(); // accept�� �Ͼ�� �������� ���� �����
					AppendText("���ο� ������ from " + client_socket);
					// User �� �ϳ��� Thread ����
					UserService new_user = new UserService(client_socket);
					UserServiceVec.add(new_user); // ���ο� ������ �迭�� �߰�

					new_user.start(); // ���� ��ü�� ������ ����
					AppendText("���� ������ �� " + UserServiceVec.size());
				} catch (IOException e) {
					AppendText("accept() error");
					// System.exit(0);

				}
			}
		}
	}

	public void AppendText(String str) {
		// textArea.append("����ڷκ��� ���� �޼��� : " + str+"\n");
		textArea.append(str + "\n");
		textArea.setCaretPosition(textArea.getText().length());
	}

	// User �� �����Ǵ� Thread
	// Read One ���� ��� -> Write All
	class UserService extends Thread {

		private ObjectInputStream ois;
		private ObjectOutputStream oos;

		private Socket client_socket;
		private Vector<UserService> user_service_vc;
		private Vector<Integer> us_room_vc = new Vector<Integer>();
		public User user;

		public UserService(Socket client_socket) {
			// TODO Auto-generated constructor stub
			// �Ű������� �Ѿ�� �ڷ� ����
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

		// ��� User�鿡�� Object�� ���. ä�� message�� image object�� ���� �� �ִ�
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
				} // e1 catch�� ����
				Logout(); // �������� ���� ��ü�� ���Ϳ��� �����
			} // e catch�� ����
		} // WriteMsg �Լ� ����

		// Windows ó�� message ������ ������ �κ��� NULL �� ����� ���� �Լ�
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

		public void CreateRoom(Msg msg) { // room_id ����

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

			// ���������鿡�� �ű� ���� ����
			for (int i = 0; i < r.uslist.size(); i++) {
				UserService us = r.uslist.elementAt(i);
				Msg new_msg = new Msg(us.user, 101, msg.room_id);
				new_msg.UserVec = r.ulist;
				us.WriteOneObject(new_msg);
			}

			// �ű��������� ä�ù� ����
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
			while (true) { // ����� ������ ����ؼ� �ޱ� ���� while��
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
						Logout(); // �������� ���� ��ü�� ���Ϳ��� �����
						break;
					} catch (Exception ee) {
						break;
					} // catch�� ��
				} // �ٱ� catch�� ��
			} // while ��
		} // run ��
	} // Userservice ��

}
