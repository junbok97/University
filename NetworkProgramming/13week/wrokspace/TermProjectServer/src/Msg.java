import java.io.Serializable;
import java.util.Vector;

import javax.swing.ImageIcon;

public class Msg implements Serializable {
	private static final long serialVersionUID = 1L;
	public User user;
	public int code; // 100 ä�ù� ���� 101 ä�ù� ���� ������Ʈ 200:ä�ø޽���, 300:Image,
	public int room_id;
	private String data;
	public ImageIcon img;
	public Vector<User> UserVec = null;

	public Msg(User user, int code, int room_id) {
		this.user = user;
		this.code = code;
		this.room_id = room_id;
	}

	public Msg(User user, int code) {
		this(user, code, -1);
	}

	public String getData() {
		return data;
	}

	public void setData(String data) {
		this.data = data;
	}

	public void setImg(ImageIcon img) {
		this.img = img;
	}

}
