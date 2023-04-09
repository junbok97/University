import java.awt.BorderLayout;
import java.awt.Color;

import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.SwingConstants;

public class Chat extends JPanel {

	JLabel profile;
	JLabel Msg;

	public Chat() {
		setBackground(Color.WHITE);
		setLayout(new BorderLayout(0, 0));

	}

	public void setData(Msg msg) {
		profile = new JLabel(msg.user.name, msg.user.profile, SwingConstants.LEFT);
		add(profile, BorderLayout.NORTH);
		Msg = new JLabel(msg.getData(), SwingConstants.LEFT);
		add(Msg, BorderLayout.CENTER);
	}

	public void setImg(Msg msg) {
		profile = new JLabel(msg.user.name, msg.user.profile, SwingConstants.LEFT);
		add(profile, BorderLayout.NORTH);
		Msg = new JLabel(msg.img, SwingConstants.LEFT);
		add(Msg, BorderLayout.CENTER);
	}

	public void setDataMe(Msg msg) {
		profile = new JLabel(msg.user.name, msg.user.profile, SwingConstants.RIGHT);
		add(profile, BorderLayout.NORTH);
		Msg = new JLabel(msg.getData(), SwingConstants.RIGHT);
		add(Msg, BorderLayout.CENTER);
	}

	public void setImgMe(Msg msg) {
		profile = new JLabel(msg.user.name, msg.user.profile, SwingConstants.RIGHT);
		add(profile, BorderLayout.NORTH);
		Msg = new JLabel(msg.img, SwingConstants.RIGHT);
		add(Msg, BorderLayout.CENTER);
	}

}
