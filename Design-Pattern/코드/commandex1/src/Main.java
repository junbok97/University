public class Main {
    public static void main(String[] args) {
        TV tv = new TV();
        Command powerCmd = new PowerCommand(tv);
        Command muteCmd = new MuteCommand(tv);
        TwoButtonController rc = new TwoButtonController(muteCmd, powerCmd);

        rc.button1Pressed();
        rc.button2Pressed();

    }
}
