import java.awt.BorderLayout;
import java.awt.GridLayout;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.ScrollPaneConstants;

public class EmoticonView extends JFrame {
	int room_id;
	MainView mainview;
	JPanel panel;

	public EmoticonView(MainView mainview) {
		this.mainview = mainview;
		setSize(400, 700);

		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setHorizontalScrollBarPolicy(ScrollPaneConstants.HORIZONTAL_SCROLLBAR_NEVER);
		scrollPane.setVerticalScrollBarPolicy(ScrollPaneConstants.VERTICAL_SCROLLBAR_ALWAYS);
		getContentPane().add(scrollPane, BorderLayout.CENTER);

		panel = new JPanel();
		scrollPane.setViewportView(panel);
		panel.setLayout(new GridLayout(0, 4, 0, 0));
		MakeEmoticon();

	}
	
	public void open(Point point,int room_id) {
		
		point.x += 394;
		this.setLocation(point);
		this.setVisible(true);
		this.room_id = room_id;
	}
	

	public void MakeEmoticon() {
		for (int i = 1; i <= 10; i++) {
			ImageIcon ori_icon = new ImageIcon("image/emoticon/상상부기" + i + ".png");
			Image ori_img = ori_icon.getImage();
			Image new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
			ImageIcon new_icon = new ImageIcon(new_img);
			JButton btn = new JButton(new_icon);
			btn.addActionListener(new EmoticonAction());
			panel.add(btn);
		}
	}

	class EmoticonAction implements ActionListener {

		public void actionPerformed(ActionEvent e) {
			JButton btn = (JButton) e.getSource();
			ImageIcon icon = (ImageIcon) btn.getIcon();
			Msg msg = new Msg(mainview.user, 400, room_id);
			msg.setImg(icon);
			mainview.SendObject(msg);
		}
	}

}
