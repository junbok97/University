import javax.swing.JFrame;
import java.awt.FlowLayout;
import java.awt.Point;

import javax.swing.JTextPane;


public class JavaObjClientListView extends JFrame {		
	JTextPane userlist;	

	public JavaObjClientListView(Point point) {		
		setPoint(point);
		setTitle("UserList");
		setDefaultCloseOperation(JFrame.DISPOSE_ON_CLOSE);
		setBounds(100, 100, 394, 630);
		setVisible(false);
		getContentPane().setLayout(new FlowLayout(FlowLayout.CENTER, 5, 5));
		
		userlist = new JTextPane();		
		userlist.setEditable(false);
		getContentPane().add(userlist);
	}
	
	public void setPoint(Point point) {
		point.x += 394;
		this.setLocation(point);
	}
	
	public void setText(String msg) {
		userlist.setText(msg);	
	}

}