import java.awt.Color;
import java.awt.EventQueue;
import java.awt.FileDialog;
import java.awt.Frame;
import java.awt.Image;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

import javax.swing.ImageIcon;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JLabel;
import javax.swing.JTextField;
import javax.swing.SwingConstants;

public class LoginView extends JFrame {
	private static final long serialVersionUID = 1L;
	private JTextField txtName;
	private Frame frame;
	private FileDialog fd;
	private JLabel lbUserProfile;

	
	
	public static void main(String[] args) {
		EventQueue.invokeLater(new Runnable() {
			public void run() {
				try {
					LoginView login = new LoginView();
					login.setVisible(true);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		});
	}
	

	public LoginView(){
		getContentPane().setBackground(new Color(255, 255, 0));		
		setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		setResizable(false);
		setSize(400,700);
		getContentPane().setLayout(null);
		
		JButton btnProfile = new JButton("Profile");
		btnProfile.setBounds(148, 334, 100, 30);
		getContentPane().add(btnProfile);		
		
		ImageIcon ori_icon = new ImageIcon("image/상상부기1.png");
		Image ori_img = ori_icon.getImage();
		Image new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
		ImageIcon new_icon = new ImageIcon(new_img);
		
		lbUserProfile = new JLabel(new_icon);
		lbUserProfile.setBounds(148, 205, 100, 100);
		getContentPane().add(lbUserProfile);
		
		txtName = new JTextField();
		txtName.setBounds(130, 410, 150, 40);
		getContentPane().add(txtName);
		txtName.setColumns(10);
		
		JLabel lbUserName = new JLabel("Name");
		lbUserName.setHorizontalAlignment(SwingConstants.CENTER);
		lbUserName.setBounds(52, 410, 70, 40);
		getContentPane().add(lbUserName);
		
		JButton btnLogin = new JButton("Login");
		btnLogin.setBounds(130, 472, 150, 40);		
		getContentPane().add(btnLogin);		
		
		ProfileAction profileAction = new ProfileAction();
		btnProfile.addActionListener(profileAction);
		
		LoginAction loginAction = new LoginAction();
		btnLogin.addActionListener(loginAction);
		
	}
	
	class ProfileAction implements ActionListener{
		@Override
		public void actionPerformed(ActionEvent e) {			
				frame = new Frame("이미지 첨부");
				fd = new FileDialog(frame, "이미지 선택", FileDialog.LOAD);				
				fd.setVisible(true);
				ImageIcon ori_icon = new ImageIcon(fd.getDirectory() + fd.getFile());
				Image ori_img = ori_icon.getImage();
				Image new_img = ori_img.getScaledInstance(50, 50, Image.SCALE_SMOOTH);
				
				ImageIcon new_icon = new ImageIcon(new_img);
				lbUserProfile.setIcon(new_icon);		
		
		}
	}
	
	class LoginAction implements ActionListener{
		@Override
		public void actionPerformed(ActionEvent e) {
			User user = new User(txtName.getText(),(ImageIcon)lbUserProfile.getIcon());
			MainView view = new MainView(user);
			setVisible(false);
		}
	}
	
	
}