public class BinarySubscriber implements Subscriber {
    private HansungBlogger blogger;

    public BinarySubscriber(HansungBlogger blogger) {
        this.blogger = blogger;
    }

    @Override
    public void update() {
        int state = blogger.getState();
        //2진수로 변환하여 출력 (앞에 0을 임의적으로 패딩하지 말 것)
        //예 5=>101(바른 변환)로 변환 5=>0101(틀린 변환)
        System.out.print(Integer.toBinaryString(state));
    }
}