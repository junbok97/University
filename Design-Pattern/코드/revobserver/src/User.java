public class User implements Observer {
    private String name;
    private Blog myBlog;

    public void setMyBlog(Blog myBlog) {
        this.myBlog = myBlog;
    }

    public User(String name) {
        this.name = name;
    }

    @Override
    public void update() {
        String content = myBlog.getContent();
        System.out.print("User: "+content);
    }
}
