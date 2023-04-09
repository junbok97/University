public class OctalSubscriber implements Subscriber {
    private HansungBlogger blogger;

    public OctalSubscriber(HansungBlogger blogger) {
        this.blogger = blogger;
    }

    @Override
    public void update() {
        int state = blogger.getState();
        //8진수로 변환하여 출력
        System.out.print(Integer.toOctalString(state));
    }
}
