import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.Component;
import java.awt.Font;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.net.Socket;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.DefaultListModel;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTabbedPane;
import javax.swing.ListCellRenderer;
import javax.swing.ListSelectionModel;
import javax.swing.SwingConstants;
import javax.swing.UIManager;

public class MainView extends JFrame {

	private MainView mainview;
	private SelectView selectview;
	public User user;
	private String ip_addr = "127.0.0.1";
	private String port_no = "40000";
	private Socket socket; // 연결소켓

	private ObjectInputStream ois;
	private ObjectOutputStream oos;

	private JTabbedPane tabbedPane;

	private JPanel tab_userlist;
	private JScrollPane sp_MyFriendList;
	private JLabel lb_friendlist;
	private DefaultListModel<ImageIcon> MyFriendListModel = new DefaultListModel<ImageIcon>();
	private JList<ImageIcon> MyFriendList = new JList<ImageIcon>();
	private CustomFriendListRenderer MyFriendRender = new CustomFriendListRenderer();

	private Font font = new Font("맑은 고딕", Font.BOLD, 20);

	// JList에 JLabel 렌더링
	public class CustomFriendListRenderer extends JLabel implements ListCellRenderer<Object> {
		public CustomFriendListRenderer() {
			setOpaque(true);
		}

		@Override
		public Component getListCellRendererComponent(JList<?> list, Object value, int index, boolean isSelected,
				boolean cellHasFocuse) {
			setIcon((ImageIcon) value);
			setText(value.toString());

			if (isSelected) {
				setBackground(list.getSelectionBackground());
				setForeground(list.getSelectionForeground());
			} else {
				setBackground(list.getBackground());
				setForeground(list.getForeground());
			}

			return this;
		}
	}

	private JPanel tab_chatlist;
	private JScrollPane sp_MyChatList;
	private JLabel lb_chatlist;
	/*
	 * private DefaultListModel<String> MyChatListModel = new
	 * DefaultListModel<String>(); private JList<String> MyChatList = new
	 * JList<String>();
	 */

	private DefaultListModel<ChatView> ChatViewModel = new DefaultListModel<ChatView>();
	private JList<ChatView> ChatViewList = new JList<ChatView>(ChatViewModel);
	private CustomChatListRenderer MyChatRender = new CustomChatListRenderer();

	// JList에 JLabel 렌더링
	public class CustomChatListRenderer extends JLabel implements ListCellRenderer<Object> {
		public CustomChatListRenderer() {
			setOpaque(true);
		}

		@Override
		public Component getListCellRendererComponent(JList<?> list, Object value, int index, boolean isSelected,
				boolean cellHasFocuse) {
			setText(value.toString());

			if (isSelected) {
				setBackground(list.getSelectionBackground());
				setForeground(list.getSelectionForeground());
			} else {
				setBackground(list.getBackground());
				setForeground(list.getForeground());
			}

			this.setFont(font);

			return this;
		}
	}

	private Vector<User> UserVec = new Vector<User>();
	private Vector<ChatView> ChatViewVec = new Vector<ChatView>();
	private JPanel p_chat;

	public MainView(User user) {
		this.mainview = this;
		this.user = user;
		selectview = new SelectView(UserVec, mainview, ChatViewVec);
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		setSize(400, 700);
		getContentPane().setLayout(new BorderLayout(0, 0));

		tabbedPane = new JTabbedPane(JTabbedPane.LEFT);
		getContentPane().add(tabbedPane, BorderLayout.CENTER);

		ImageIcon ori_icon = new ImageIcon("image/icon/person.png");
		Image ori_img = ori_icon.getImage();
		Image new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		ImageIcon new_icon = new ImageIcon(new_img);

		tab_userlist = new JPanel();
		tabbedPane.addTab(null, new_icon, tab_userlist, "친구목록");
		tab_userlist.setLayout(null);

		ori_icon = new ImageIcon("image/icon/chat.png");
		ori_img = ori_icon.getImage();
		new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		new_icon = new ImageIcon(new_img);

		tab_chatlist = new JPanel();
		tabbedPane.addTab(null, new_icon, tab_chatlist, "채팅목록");
		tab_chatlist.setLayout(null);

		p_chat = new JPanel();
		p_chat.setBounds(0, 0, 372, 668);
		tab_chatlist.add(p_chat);
		p_chat.setLayout(null);

		JPanel p_profile = new JPanel();
		p_profile.setLayout(null);
		p_profile.setBackground(UIManager.getColor("Button.background"));
		p_profile.setBounds(0, 0, 372, 668);
		tab_userlist.add(p_profile);

		JLabel lb_friend = new JLabel("친구");
		lb_friend.setHorizontalAlignment(SwingConstants.CENTER);
		lb_friend.setFont(font);
		lb_friend.setBounds(0, 0, 100, 60);
		p_profile.add(lb_friend);

		JLabel lb_profile_Image = new JLabel(user.profile);
		lb_profile_Image.setHorizontalAlignment(SwingConstants.CENTER);
		lb_profile_Image.setBounds(30, 65, 50, 50);
		p_profile.add(lb_profile_Image);

		JLabel lb_profile_Name = new JLabel(user.name);
		lb_profile_Name.setHorizontalAlignment(SwingConstants.CENTER);
		lb_profile_Name.setBounds(80, 65, 100, 50);
		p_profile.add(lb_profile_Name);

		JButton btnSelectView = new JButton("+");
		btnSelectView.setBackground(UIManager.getColor("Button.background"));
		btnSelectView.setBounds(238, 13, 45, 45);
		p_chat.add(btnSelectView);

		JLabel lb_Chat = new JLabel("채팅");
		lb_Chat.setBounds(0, 0, 100, 60);
		p_chat.add(lb_Chat);
		lb_Chat.setFont(font);
		lb_Chat.setHorizontalAlignment(SwingConstants.CENTER);

		sp_MyChatList = new JScrollPane();
		sp_MyChatList.setBounds(0, 85, 372, 583);

		p_chat.add(sp_MyChatList);

		/*
		 * MyChatList.setLayoutOrientation(JList.HORIZONTAL_WRAP);
		 * MyChatList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		 * MyChatList.setBackground(UIManager.getColor("Button.background"));
		 * MyChatList.setModel(MyChatListModel);
		 * MyChatList.setCellRenderer(MyChatRender);
		 * sp_MyChatList.setViewportView(MyChatList);
		 */

		// ChatViewList.setLayoutOrientation(JList.HORIZONTAL_WRAP);
		ChatViewList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		ChatViewList.setBackground(UIManager.getColor("Button.background"));
		ChatViewList.setCellRenderer(MyChatRender);
		sp_MyChatList.setViewportView(ChatViewList);

		lb_chatlist = new JLabel("채팅방 목록 (0)");
		lb_chatlist.setBackground(Color.WHITE);
		sp_MyChatList.setColumnHeaderView(lb_chatlist);

		sp_MyFriendList = new JScrollPane();
		sp_MyFriendList.setEnabled(false);
		sp_MyFriendList.setBounds(0, 154, 372, 516);
		p_profile.add(sp_MyFriendList);

		// MyFriendList.setLayoutOrientation(JList.HORIZONTAL_WRAP);
		MyFriendList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		MyFriendList.setBackground(UIManager.getColor("Button.background"));
		MyFriendList.setModel(MyFriendListModel);
		MyFriendList.setCellRenderer(MyFriendRender);
		sp_MyFriendList.setViewportView(MyFriendList);
		MyFriendList.setSelectionMode(ListSelectionModel.SINGLE_SELECTION);
		MyFriendList.addMouseListener(new JListMouseEvent());

		lb_friendlist = new JLabel("친구 목록 (0)");
		lb_friendlist.setBackground(Color.WHITE);
		sp_MyFriendList.setColumnHeaderView(lb_friendlist);

		btnSelectView.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				selectview.open(getLocation());
			}
		});

		try {
			socket = new Socket(ip_addr, Integer.parseInt(port_no));
			oos = new ObjectOutputStream(socket.getOutputStream());
			oos.flush();
			ois = new ObjectInputStream(socket.getInputStream());
			SendObject(user);
			ListenNetwork net = new ListenNetwork();
			net.start();

		} catch (NumberFormatException | IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		} // try catch 끝
		setVisible(true);
	} // 생성자 끝

	class JListMouseEvent extends MouseAdapter {
		public void mouseClicked(MouseEvent evt) {
			if (evt.getClickCount() == 2) {

				for (int i = 0; i < ChatViewVec.size(); i++) {
					ChatView cv = (ChatView) ChatViewVec.elementAt(i);
					if(cv.userlist.size() == 2) {
						for (int j = 0; j < 2; j++) {
							User u = (User) cv.userlist.elementAt(j);
							if (u.name.equals(user.name)) {
								cv.open(getLocation());
								return;
							}
						}
					}					
				}

				Msg msg = new Msg(user, 100);
				Vector<User> uv = new Vector<User>();

				ImageIcon icon = new ImageIcon(MyFriendListModel.get(MyFriendList.getSelectedIndex()).getImage());
				User u = new User(MyFriendListModel.get(MyFriendList.getSelectedIndex()).getDescription(), icon);

				uv.add(user);
				uv.add(u);

				msg.UserVec = uv;
				SendObject(msg);
			}
		}
	}

	// Server Message를 수신해서 화면에 표시
	class ListenNetwork extends Thread {
		public void run() {
			while (true) {
				try {
					Object obcm = null;
					Userlist ul;
					Msg msg;
					try {
						obcm = ois.readObject();
					} catch (ClassNotFoundException e) {
						// TODO Auto-generated catch block
						e.printStackTrace();
						break;
					}
					if (obcm == null)
						break;
					if (obcm instanceof Userlist) {
						ul = (Userlist) obcm;
						UpdateFriendList(ul);
					}
					if (obcm instanceof Msg) {
						msg = (Msg) obcm;
						switch (msg.code) {
						case 100:
							CreateChat(msg);
							break;
						case 200:
							AppendChatMsg(msg);
							break;
						}

					} else
						continue;

				} catch (IOException e) {
					try {
//								dos.close();
//								dis.close();
						ois.close();
						oos.close();
						socket.close();

						break;
					} catch (Exception ee) {
						break;
					} // catch문 끝
				} // 바깥 catch문끝

			} // while문 끝
		} // run() 끝
	} // ListenNetwork class 끝

	public void CreateChat(Msg msg) {
		ChatView cv = new ChatView(user, mainview, selectview, msg.room_id, msg.UserVec);
		ChatViewVec.add(cv);
		cv.open(getLocation());
		ChatViewModel.addElement(cv);

		/*
		 * String str = Integer.toString(msg.room_id); MyChatListModel.addElement(str);
		 * MyChatList.setModel(MyChatListModel);
		 */
	}

	public void UpdateFriendList(Userlist ul) {
		MyFriendListModel.removeAllElements();
		UserVec.removeAllElements();

		Iterator<User> it = ul.userlist.iterator();
		User u;

		for (int i = 0; i < ul.userlist.size(); i++) {
			u = ul.userlist.elementAt(i);
			if (u.name.equals(this.user.name))
				ul.userlist.remove(i);
		}

		for (int i = 0; i < ul.userlist.size(); i++) {
			u = ul.userlist.elementAt(i);
			MyFriendListModel.addElement(new ImageIcon(u.profile.getImage(), u.name));
		}

		this.UserVec = ul.userlist;

		lb_friendlist.setText("친구 목록 (" + UserVec.size() + ")");
		selectview.update(UserVec);
	}

	public void AppendChatMsg(Msg msg) {

		for (int i = 0; i < ChatViewVec.size(); i++) {
			ChatView chat_view = ChatViewVec.elementAt(i);
			if (msg.room_id == chat_view.room_id) {
				chat_view.open(getLocation());
				switch (msg.code) {
				case 200:
					chat_view.AppendText(msg);
					break;
				case 300:
					chat_view.AppendImage(msg);
					break;
				}
			}
		}

	}

	public void SendObject(Object ob) { // 서버로 메세지를 보내는 메소드
		try {
			oos.writeObject(ob);
			oos.reset();
		} catch (IOException e) {
		}
	}
}
