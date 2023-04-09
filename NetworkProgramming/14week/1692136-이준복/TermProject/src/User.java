import java.io.Serializable;

import javax.swing.ImageIcon;

public class User implements Serializable{
	
	public String name;
	public ImageIcon profile;			
	
	public User(String name, ImageIcon profile) {
		this.name = name;
		this.profile = profile;		
	}
	
}