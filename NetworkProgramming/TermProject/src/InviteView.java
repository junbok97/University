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

public class InviteView extends JFrame {

	private MainView view;
	private ChatView chatview;
	private JScrollPane scrollPane;	
	private Vector<JCheckBox> CBVec;
	


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

	public InviteView(MainView view, ChatView chatview) {	
		this.view = view;
		this.chatview = chatview;
		setResizable(false);
		setSize(400, 700);
		getContentPane().setLayout(new BorderLayout(0, 0));

		scrollPane = new JScrollPane();
		scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		getContentPane().add(scrollPane, BorderLayout.CENTER);
		
		JPanel panel = new JPanel();
		scrollPane.setViewportView(panel);
		
		JLabel lb_title = new JLabel("\uB300\uD654\uC0C1\uB300 \uCD08\uB300");
		lb_title.setFont(new Font("±¼¸²", Font.BOLD, 20));
		lb_title.setHorizontalAlignment(SwingConstants.CENTER);
		lb_title.setBorder(null);
		
		scrollPane.setColumnHeaderView(lb_title);

		JButton btn_Make = new JButton("\uC720\uC800 \uCD08\uB300");
		getContentPane().add(btn_Make, BorderLayout.SOUTH);
		btn_Make.addActionListener(new MakeChatAction());

	}

	public void open() {
		Point point;
		point = chatview.getLocation();
		point.x += 394;
		this.setLocation(point);
		this.setVisible(true);
	}
	

	public void userupdate() {
	
		JPanel p = new JPanel();
		CBVec = new Vector<JCheckBox>();
		p.setLayout(new BoxLayout(p, BoxLayout.Y_AXIS));
		
		Boolean check;
		
		for(int i = 0; i < view.UserVec.size(); i++) {
			
			check = true;
			User u1 = (User)view.UserVec.elementAt(i);
			
			for(int j =0 ; j < chatview.userlist.size();j++) {
				User u2 = (User) chatview.userlist.elementAt(j);
				if(u1.name.equals(u2.name)) {
					check = false;
					break;
				}
			}
			
			if(check) {
				JCheckBox jcb = new JCheckBox(u1.name, u1.profile);
				jcb.setBorderPainted(true);
				p.add(jcb);
				CBVec.add(jcb);				
			}			
		}		

		scrollPane.setViewportView(p);
		revalidate();
		repaint();
	}

	class MakeChatAction implements ActionListener {

		@Override
		public void actionPerformed(ActionEvent e) {
			
			Vector<User> userlist = new Vector<User>();		
			
			for (int i = 0; i < CBVec.size(); i++) {
				JCheckBox jcb = (JCheckBox) CBVec.elementAt(i);
				if (jcb.isSelected()) {
					User u = new User(jcb.getText(), (ImageIcon) jcb.getIcon());
					userlist.add(u);
				}
			}			

			Msg msg = new Msg(view.user, 101,chatview.room_id);
			msg.UserVec = userlist;			
			view.SendObject(msg);
		}
	}

}
