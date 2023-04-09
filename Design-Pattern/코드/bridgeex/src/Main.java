public class Main {
    public static void main(String[] args) {
        Tv tv = new Tv();
        BasicRemote r1 = new BasicRemote(tv);
        r1.power();
        r1.channelUp();
        tv.printStatus();

        Radio radio = new Radio();
        AdvanceRemoteWithMute r2 = new AdvanceRemoteWithMute(radio);
        r2.power();
        r2.volumeUp();
        radio.printStatus();
        r2.mute();
        radio.printStatus();
    }
}
