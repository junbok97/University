public class Main {
    public static void main(String[] args) {
        HansungBlogger h1 = new HansungBlogger();
        HansungBlogger h2 = new HansungBlogger();
        Subscriber m1 = new BinarySubscriber(h1);
        Subscriber m2 = new HexaSubscriber(h2);
        Subscriber m3 = new OctalSubscriber(h1);

        h1.join(m1);
        h2.join(m2);
        h1.join(m3);

        h1.setState(10);
        h2.setState(5);
        h1.setState(10);
    }
}
