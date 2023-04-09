import java.awt.BorderLayout;
import java.awt.Component;
import java.awt.Font;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.util.Vector;

import javax.swing.BoxLayout;
import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JCheckBox;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JList;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ListCellRenderer;
import javax.swing.ScrollPaneConstants;
import javax.swing.SwingConstants;

public class SelectView extends JFrame {

	private MainView view;
	private JScrollPane scrollPane;
	private JPanel panel;
	private Vector<JCheckBox> CBVec;
	private Vector<ChatView> ChatViewVec;

	/*
	 * private DefaultListModel<ImageIcon> MyFriendListModel = new
	 * DefaultListModel<ImageIcon>(); private JList<ImageIcon> MyFriendList = new
	 * JList<ImageIcon>(); private CustomFriendListRenderer MyFriendRender = new
	 * CustomFriendListRenderer();
	 */

	// JList¿¡ JLabel ·»´õ¸µ
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

	public SelectView(Vector<User> UserVec, MainView view, Vector<ChatView> ChatViewVec) {	
		this.view = view;
		this.ChatViewVec = ChatViewVec;
		setResizable(false);
		setSize(400, 700);
		getContentPane().setLayout(new BorderLayout(0, 0));

		scrollPane = new JScrollPane();
		scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		getContentPane().add(scrollPane, BorderLayout.CENTER);

		/*
		 * MyFriendList.setSelectionMode(ListSelectionModel.MULTIPLE_INTERVAL_SELECTION)
		 * ; MyFriendList.setBackground(UIManager.getColor("Button.background"));
		 * MyFriendList.setModel(MyFriendListModel);
		 * MyFriendList.setCellRenderer(MyFriendRender);
		 * scrollPane.setViewportView(MyFriendList);
		 */

		UpdateUser(UserVec);
		scrollPane.setViewportView(panel);
		
		JLabel lb_title = new JLabel("\uB300\uD654\uC0C1\uB300 \uC120\uD0DD");
		lb_title.setFont(new Font("±¼¸²", Font.BOLD, 20));
		lb_title.setHorizontalAlignment(SwingConstants.CENTER);
		lb_title.setBorder(null);
		
		scrollPane.setColumnHeaderView(lb_title);

		JButton btn_Make = new JButton("\uCC44\uD305\uBC29 \uB9CC\uB4E4\uAE30");
		getContentPane().add(btn_Make, BorderLayout.SOUTH);
		btn_Make.addActionListener(new MakeChatAction());

	}

	public void open(Point point) {
		point.x -= 394;
		this.setLocation(point);
		this.setVisible(true);
	}
	

	public void UpdateUser(Vector<User> UserVec) {
		/*
		 * MyFriendListModel.removeAllElements(); Iterator<User> it =
		 * UserVec.iterator(); while (it.hasNext()) { User u = it.next();
		 * MyFriendListModel.addElement(new ImageIcon(u.profile.getImage(), u.name)); }
		 */

		JPanel p = new JPanel();
		CBVec = new Vector<JCheckBox>();
		p.setLayout(new BoxLayout(p, BoxLayout.Y_AXIS));
		for (int i = 0; i < UserVec.size(); i++) {
			User u = (User) UserVec.elementAt(i);
			JCheckBox jcb = new JCheckBox(u.name, u.profile);
			jcb.setBorderPainted(true);
			p.add(jcb);
			CBVec.add(jcb);
		}
		panel = p;
		scrollPane.setViewportView(panel);

	}

	class MakeChatAction implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			Vector<User> userlist = new Vector<User>();
			userlist.add(view.user);
			for (int i = 0; i < CBVec.size(); i++) {
				JCheckBox jcb = (JCheckBox) CBVec.elementAt(i);
				if (jcb.isSelected()) {
					User u = new User(jcb.getText(), (ImageIcon) jcb.getIcon());
					userlist.add(u);
				}
			}

			int count;
			for (int i = 0; i < ChatViewVec.size(); i++) {
				count = 0;
				ChatView cv = (ChatView) ChatViewVec.elementAt(i);
				for (int j = 0; j < cv.userlist.size(); j++) {
					User u1 = (User) cv.userlist.elementAt(j);
					for (int k = 0; k < userlist.size(); k++) {
						User u2 = (User) userlist.elementAt(k);
						if (u2.name.equals(u1.name)) {
							count++;
							break;
						}
					}
					if (count == userlist.size()) {
						cv.open(getLocation());					
						return;
					}
				}
			}

			Msg msg = new Msg(view.user, 100);
			msg.UserVec = userlist;
			view.SendObject(msg);
		}
	}

}
