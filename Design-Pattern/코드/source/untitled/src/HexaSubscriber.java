
public class HexaSubscriber implements Subscriber {
    private HansungBlogger blogger;

    public HexaSubscriber(HansungBlogger blogger) {
        this.blogger = blogger;
    }

    @Override
    public void update() {
        int state = blogger.getState();
        //16진수로 변환하여 출력
        System.out.print(Integer.toHexString(state));
    }
}