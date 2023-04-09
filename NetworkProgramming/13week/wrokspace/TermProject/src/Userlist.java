import java.io.Serializable;
import java.util.Vector;

public class Userlist implements Serializable{

	public Vector<User> userlist;
	
	public Userlist(Vector<User> userlist) {
		this.userlist = userlist;
	}
	
}
