import java.awt.Color;
import java.awt.FileDialog;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Image;
import java.awt.Point;
import java.awt.SystemColor;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JPanel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.SwingConstants;
import javax.swing.text.StyledDocument;

public class ChatView extends JFrame {
	public User user;
	public int room_id;
	private JTextField txtInput;
	JButton btn_send;
	JButton btn_emoticon;
	JButton btn_image;
	JButton btn_invite;
	JTextPane textArea;
	Frame frame;
	FileDialog fd;
	MainView mainview;
	SelectView selectview;
	Vector<User> userlist;
	JLabel chat_name;

	InviteView inviteview;
	EmoticonView emoticonview;

	Color backcolor = SystemColor.activeCaption;

	@Override
	public String toString() {
		String str = "[" + Integer.toString(room_id) + "]";

		str = str + " " + userlist.elementAt(0).name;

		for (int i = 1; i < userlist.size(); i++) {
			str = str + ", " + userlist.elementAt(i).name;
		}

		return str;
	}

	public ChatView(User user, MainView mainview, EmoticonView emoticonview, SelectView selectview, int room_id,
			Vector<User> userlist) {
		getContentPane().setBackground(backcolor);
		this.user = user;
		this.mainview = mainview;
		this.emoticonview = emoticonview;
		this.selectview = selectview;
		this.room_id = room_id;
		this.userlist = userlist;

		setResizable(false);
		setSize(400, 700);
		getContentPane().setLayout(null);

		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBorder(null);
		scrollPane.setBackground(backcolor);
		scrollPane.setBounds(12, 53, 372, 487);
		getContentPane().add(scrollPane);

		textArea = new JTextPane();
		textArea.setBackground(backcolor);
		textArea.setBorder(null);
		textArea.setEnabled(false);
		scrollPane.setViewportView(textArea);

		ImageIcon ori_icon = new ImageIcon("image/icon/emoticon.png");
		Image ori_img = ori_icon.getImage();
		Image new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		ImageIcon new_icon = new ImageIcon(new_img);

		chat_name = new JLabel(toString());
		chat_name.setBackground(Color.WHITE);
		chat_name.setFont(new Font("굴림", Font.BOLD, 15));
		chat_name.setHorizontalAlignment(SwingConstants.CENTER);
		chat_name.setBounds(12, 10, 372, 33);
		getContentPane().add(chat_name);

		JPanel panel = new JPanel();
		panel.setBackground(new Color(255, 255, 255));
		panel.setBounds(0, 550, 396, 123);
		getContentPane().add(panel);
		panel.setLayout(null);

		txtInput = new JTextField();
		txtInput.setBounds(10, 6, 282, 54);
		txtInput.setBorder(null);
		panel.add(txtInput);
		txtInput.setColumns(10);

		ori_icon = new ImageIcon("image/icon/invite.png");
		ori_img = ori_icon.getImage();
		new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		new_icon = new ImageIcon(new_img);
		btn_invite = new JButton(new_icon);
		btn_invite.setBackground(new Color(255, 255, 255));
		btn_invite.setOpaque(true);
		btn_invite.setBounds(10, 70, 40, 40);
		btn_invite.setBorderPainted(false);
		panel.add(btn_invite);

		ori_icon = new ImageIcon("image/icon/emoticon.png");
		ori_img = ori_icon.getImage();
		new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		new_icon = new ImageIcon(new_img);
		btn_emoticon = new JButton(new_icon);
		btn_emoticon.setBackground(new Color(255, 255, 255));
		btn_emoticon.setBounds(60, 70, 40, 40);
		btn_emoticon.setOpaque(true);
		btn_emoticon.setBorderPainted(false);
		panel.add(btn_emoticon);

		ori_icon = new ImageIcon("image/icon/image.png");
		ori_img = ori_icon.getImage();
		new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		new_icon = new ImageIcon(new_img);
		btn_image = new JButton(new_icon);
		btn_image.setBackground(new Color(255, 255, 255));
		btn_image.setBounds(110, 70, 40, 40);
		btn_image.setOpaque(true);
		btn_image.setBorderPainted(false);
		panel.add(btn_image);

		btn_send = new JButton("전송");
		btn_send.setBounds(304, 10, 80, 50);
		btn_send.setOpaque(true);
		panel.add(btn_send);
		btn_send.setBackground(new Color(255, 255, 0));

		JButton btnExit = new JButton("종료");
		btnExit.setBounds(304, 70, 80, 40);
		btnExit.setOpaque(true);
		panel.add(btnExit);
		btnExit.setBackground(new Color(255, 255, 0));
		btnExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				close();
			}
		});

		TextSendAction send_action = new TextSendAction();
		ImageSendAction image_action = new ImageSendAction();
		InviteAction invite_action = new InviteAction();
		EmoticonAction emoticon_action = new EmoticonAction();

		btn_emoticon.addActionListener(emoticon_action);
		btn_invite.addActionListener(invite_action);
		btn_send.addActionListener(send_action);
		btn_image.addActionListener(image_action);
		txtInput.addActionListener(send_action);
		txtInput.requestFocus();

		ReadObject();
	}

	public void make() {
		this.inviteview = new InviteView(mainview, this);
		inviteview.userupdate();
	}

	public void open(Point point) {
		point.x += 394;
		this.setLocation(point);
		this.setVisible(true);
	}

	class InviteAction implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			inviteview.open();
		}
	}

	class EmoticonAction implements ActionListener {
		public void actionPerformed(ActionEvent e) {
			emoticonview.open(getLocation(), room_id);
		}
	}

	class TextSendAction implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			// Send button을 누르거나 메시지 입력하고 Enter key 치면
			if (e.getSource() == btn_send || e.getSource() == txtInput) {
				Msg msg = new Msg(user, 200, room_id);
				msg.setData(txtInput.getText());
				mainview.SendObject(msg);
				txtInput.setText(""); // 메세지를 보내고 나면 메세지 쓰는창을 비운다.
				txtInput.requestFocus(); // 메세지를 보내고 커서를 다시 텍스트 필드로 위치시킨다
			}
		}
	}

	class ImageSendAction implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			if (e.getSource() == btn_image) {
				frame = new Frame("이미지첨부");
				fd = new FileDialog(frame, "이미지 선택", FileDialog.LOAD);
				fd.setVisible(true);
				Msg msg = new Msg(user, 300, room_id);
				String filename = fd.getDirectory() + fd.getFile();
				if (filename.matches("(.*)null(.*)"))
					return;
				ImageIcon img = new ImageIcon(filename);
				msg.setImg(img);
				mainview.SendObject(msg);
			}
		}
	}

	public void AppendText(Msg msg) {

		Chat c = new Chat(msg);

		switch (msg.code) {
		case 200:
			if (msg.user.name.equals(user.name))
				c.setMsgMe();
			else
				c.setMsg();
			break;
		case 201:
			c.Login();
			break;
		case 202:
			c.Logout();
			Logout(msg.user);
			break;
		}

		int len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.insertComponent(c);
		len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.replaceSelection("\n");
	}

	public void Logout(User user) {

		for (int i = 0; i < userlist.size(); i++) {
			User u = (User)userlist.elementAt(i);
			if(u.name.equals(user.name)) {
				userlist.remove(i);
				break;
			}				
		}		
		chat_name.setText(toString());
	}

	public void AppendImage(Msg msg) {
		int len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len); // place caret at the end (with no selection)

		Image ori_img = msg.img.getImage();
		int width, height;
		double ratio;
		width = msg.img.getIconWidth();
		height = msg.img.getIconHeight();
		// Image가 너무 크면 최대 가로 또는 세로 200 기준으로 축소시킨다.
		if (width > 200 || height > 200) {
			if (width > height) { // 가로 사진
				ratio = (double) height / width;
				width = 200;
				height = (int) (width * ratio);
			} else { // 세로 사진
				ratio = (double) width / height;
				height = 200;
				width = (int) (height * ratio);
			}
			Image new_img = ori_img.getScaledInstance(width, height, Image.SCALE_SMOOTH);
			msg.ori_img = msg.img;
			msg.setImg(new ImageIcon(new_img));
		}

		Chat c = new Chat(msg);

		if (msg.user.name.equals(user.name))
			c.setImgMe();
		else
			c.setImg();

		textArea.insertComponent(c);
		len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.replaceSelection("\n");
	}

	public void AppendEmoticon(Msg msg) {
		int len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len); // place caret at the end (with no selection)

		Chat c = new Chat(msg);

		if (msg.user.name.equals(user.name))
			c.setImgMe();
		else
			c.setImg();

		textArea.insertComponent(c);
		len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.replaceSelection("\n");
	}

	public void SaveObject() {
		try {
			File writeF = new File("./chat/" + mainview.user.name + "to" + user.name);
			if (!writeF.exists()) {
				writeF.createNewFile();
			}
			StyledDocument doc = (StyledDocument) textArea.getDocument();
			FileOutputStream fos = new FileOutputStream("./chat/" + mainview.user.name + "to" + user.name);
			ObjectOutputStream oos = new ObjectOutputStream(fos);
			oos.writeObject(doc);
			oos.flush();
			oos.close();
		} catch (IOException e) {
			e.printStackTrace();
		}
	}

	public void ReadObject() {
		try {
			File writeF = new File("./chat/" + mainview.user.name + "no" + user.name);
			if (!writeF.exists()) {
				return;
			}
			FileInputStream fis = new FileInputStream("./chat/" + mainview.user.name + "to" + user.name);
			ObjectInputStream ois = new ObjectInputStream(fis);
			StyledDocument doc = (StyledDocument) ois.readObject();
			ois.close();
			textArea.setStyledDocument(doc);
			validate();
		} catch (Exception e) {
			e.printStackTrace();
		}
	}

	public void close() {
		SaveObject();
		setVisible(false);
	}

	@Override
	public void finalize() {
		SaveObject();
	}
}
