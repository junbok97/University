import java.io.Serializable;
import java.util.Vector;

import javax.swing.ImageIcon;

public class Msg implements Serializable {
	private static final long serialVersionUID = 1L;
	public User user;	
	// 0:Login, 1:��������Ʈ, 100:ä�ù����, 101:ä�ù������ʴ�, 200:ä�ø޽���, 201:�����޼���, 202:�α׾ƿ�
	// 300:Image, 400:�̸�Ƽ��
	public int code;  
	public int room_id;
	private String data;
	public ImageIcon img;
	public ImageIcon ori_img;
	public Vector<User> UserVec = null;

	public Msg(User user, int code, int room_id) {
		this.user = user;
		this.code = code;
		this.room_id = room_id;
	}

	public Msg(User user, int code) {
		this.user = user;
		this.code = code;
	}
	
	public Msg(int code) {
		this.code = code;
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
