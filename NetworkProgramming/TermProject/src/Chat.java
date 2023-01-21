import java.awt.BorderLayout;
import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.Font;
import java.awt.SystemColor;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;
import java.text.SimpleDateFormat;
import java.util.Date;

import javax.swing.BorderFactory;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.SwingConstants;

public class Chat extends JPanel {

	JLabel lb_msg;
	JLabel lb_time;

	JLabel lb_profile;

	Date now = new Date();
	SimpleDateFormat format = new SimpleDateFormat("aa hh:mm");
	FlowLayout layout = new FlowLayout(FlowLayout.LEFT, 10, 10);
	Msg msg;

	public Chat(Msg msg) {
		this.msg = msg;
		setBackground(SystemColor.activeCaption);
		setLayout(layout);
		lb_msg = new JLabel();
		lb_msg.setOpaque(true);
		lb_time = new JLabel();
		lb_time.setFont(new Font("굴림", Font.BOLD, 10));
		lb_time.setBackground(getBackground());
		lb_time.setHorizontalAlignment(SwingConstants.CENTER);
		lb_time.setVerticalAlignment(SwingConstants.BOTTOM);

		lb_profile = new JLabel();
		lb_profile.setHorizontalTextPosition(SwingConstants.CENTER);
		lb_profile.setVerticalTextPosition(SwingConstants.TOP);

		lb_profile.setText("[" + msg.user.name + "]");
		lb_profile.setIcon(msg.user.profile);
		lb_time.setText(format.format(now));
		lb_msg.setText(msg.getData());

	}

	public void Login() {
		layout.setAlignment(FlowLayout.CENTER);
		lb_msg.setText(msg.getData() + "님이 채팅에 참여하였습니다.");
		lb_msg.setBackground(getBackground());
		add(lb_msg);
	}
	
	public void Logout() {
		layout.setAlignment(FlowLayout.CENTER);
		lb_msg.setText(msg.getData() + "님이 퇴장하였습니다.");
		lb_msg.setBackground(getBackground());
		add(lb_msg);
	}
	

	public void setMsg() {

		lb_msg.setBackground(new Color(255, 255, 255));
		lb_msg.setHorizontalAlignment(SwingConstants.LEFT);
		lb_msg.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		add(lb_profile);
		add(lb_msg);
		add(lb_time);

	}

	public void setImg() {

		lb_msg.setHorizontalAlignment(SwingConstants.LEFT);
		lb_msg.setIcon(msg.img);
		lb_msg.addMouseListener(new mouseEvent());
		add(lb_profile);
		add(lb_msg);
		add(lb_time);

	}

	public void setMsgMe() {
		layout.setAlignment(FlowLayout.RIGHT);
		lb_msg.setBorder(BorderFactory.createEmptyBorder(10, 10, 10, 10));
		lb_msg.setHorizontalAlignment(SwingConstants.RIGHT);
		lb_msg.setBackground(new Color(255, 255, 0));
		add(lb_time);
		add(lb_msg);

	}

	public void setImgMe() {
		layout.setAlignment(FlowLayout.RIGHT);
		lb_msg.setHorizontalAlignment(SwingConstants.RIGHT);
		lb_msg.setIcon(msg.img);
		lb_msg.addMouseListener(new mouseEvent());
		add(lb_time);
		add(lb_msg);
	}

	public void setEmoticon() {
		lb_msg.setHorizontalAlignment(SwingConstants.LEFT);
		lb_msg.setIcon(msg.img);
		lb_msg.addMouseListener(new mouseEvent());
		add(lb_profile);
		add(lb_msg);
		add(lb_time);
	}

	public void setEmoticonMe() {
		layout.setAlignment(FlowLayout.RIGHT);
		lb_msg.setHorizontalAlignment(SwingConstants.RIGHT);
		lb_msg.setIcon(msg.img);
		add(lb_time);
		add(lb_msg);
	}

	class mouseEvent extends MouseAdapter {
		public void mouseClicked(MouseEvent e) {

			JFrame jf = new JFrame();

			JScrollPane scrollPane = new JScrollPane();
			jf.getContentPane().add(scrollPane, BorderLayout.CENTER);

			JPanel panel = new JPanel();
			scrollPane.setViewportView(panel);

			JLabel lb = new JLabel(msg.ori_img);

			panel.add(lb);
			jf.setSize(lb.getMaximumSize());
			jf.setVisible(true);
		}
	}

}
