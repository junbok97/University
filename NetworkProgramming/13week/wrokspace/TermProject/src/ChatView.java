import java.awt.FileDialog;
import java.awt.Font;
import java.awt.Frame;
import java.awt.Image;
import java.awt.Point;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.ObjectOutputStream;
import java.util.Iterator;
import java.util.Vector;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JScrollPane;
import javax.swing.JTextField;
import javax.swing.JTextPane;
import javax.swing.SwingConstants;
import javax.swing.text.StyledDocument;

public class ChatView extends JFrame {
	public User user;
	public int room_id;
	private JTextField txtInput;
	JButton btnSend;
	JButton btnSendEmoticon;
	JButton btnSendImage;
	JButton btnInvite;
	JTextPane textArea;
	Frame frame;
	FileDialog fd;
	MainView mainview;
	SelectView selectview;
	Vector<User> userlist;
	JLabel chat_name;

	@Override
	public String toString() {
		String str = "No." + Integer.toString(room_id) + " ";
		Iterator<User> it = userlist.iterator();
		while (it.hasNext()) {
			str += it.next().name + " ";
		}
		return str;
	}

	public ChatView(User user, MainView mainview, SelectView selectview, int room_id, Vector<User> userlist) {
		this.user = user;
		this.mainview = mainview;
		this.selectview = selectview;
		this.room_id = room_id;
		this.userlist = userlist;

		setResizable(false);
		setSize(400, 700);
		getContentPane().setLayout(null);

		JScrollPane scrollPane = new JScrollPane();
		scrollPane.setBounds(12, 59, 372, 474);
		getContentPane().add(scrollPane);

		textArea = new JTextPane();
		scrollPane.setViewportView(textArea);

		txtInput = new JTextField();
		txtInput.setBounds(12, 550, 280, 50);
		getContentPane().add(txtInput);
		txtInput.setColumns(10);

		btnSend = new JButton("\uC804\uC1A1");
		btnSend.setBounds(304, 549, 80, 50);
		getContentPane().add(btnSend);

		TextSendAction Send_action = new TextSendAction();
		btnSend.addActionListener(Send_action);
		txtInput.addActionListener(Send_action);
		txtInput.requestFocus();

		ImageIcon ori_icon = new ImageIcon("image/icon/emoticon.png");
		Image ori_img = ori_icon.getImage();
		Image new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		ImageIcon new_icon = new ImageIcon(new_img);
		btnSendEmoticon = new JButton(new_icon);
		btnSendEmoticon.setBounds(62, 610, 40, 40);
		getContentPane().add(btnSendEmoticon);

		ori_icon = new ImageIcon("image/icon/image.png");
		ori_img = ori_icon.getImage();
		new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		new_icon = new ImageIcon(new_img);
		btnSendImage = new JButton(new_icon);
		btnSendImage.setBounds(114, 610, 40, 40);
		getContentPane().add(btnSendImage);
		ImageSendAction btnSendImage_action = new ImageSendAction();
		btnSendImage.addActionListener(btnSendImage_action);

		btnInvite = new JButton("+");
		btnInvite.setBounds(12, 610, 40, 40);
		getContentPane().add(btnInvite);

		JButton btnExit = new JButton("\uC885\uB8CC");
		btnExit.setBounds(304, 610, 80, 40);
		btnExit.addActionListener(new ActionListener() {
			public void actionPerformed(ActionEvent e) {
				close();
			}
		});
		getContentPane().add(btnExit);

		chat_name = new JLabel(toString());
		chat_name.setFont(new Font("굴림", Font.BOLD, 15));
		chat_name.setHorizontalAlignment(SwingConstants.CENTER);
		chat_name.setBounds(12, 10, 372, 33);
		getContentPane().add(chat_name);

		ReadObject();
	}

	public void open(Point point) {
		point.x += 394;
		this.setLocation(point);
		this.setVisible(true);
	}

	class InviteAction implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			Point point = getLocation();
			point.x += 394;
			selectview.open(point);
		}
	}

	class TextSendAction implements ActionListener {
		@Override
		public void actionPerformed(ActionEvent e) {
			// Send button을 누르거나 메시지 입력하고 Enter key 치면
			if (e.getSource() == btnSend || e.getSource() == txtInput) {
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
			if (e.getSource() == btnSendImage) {
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

		Chat c = new Chat();

		if (msg.user.name.equals(user.name))
			c.setDataMe(msg);
		else
			c.setData(msg);

		int len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.insertComponent(c);
		len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.replaceSelection("\n");
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
			msg.setImg(new ImageIcon(new_img));
		}

		Chat c = new Chat();

		if (msg.user.name.equals(user.name))
			c.setImgMe(msg);
		else
			c.setImg(msg);

		textArea.insertComponent(c);
		len = textArea.getDocument().getLength();
		textArea.setCaretPosition(len);
		textArea.replaceSelection("\n");
		// ImageViewAction viewaction = new ImageViewAction();
		// new_icon.addActionListener(viewaction); // 내부클래스로 액션 리스너를 상속받은 클래스로
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
