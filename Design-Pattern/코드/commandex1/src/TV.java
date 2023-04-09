public class TV {
    private boolean powerOn = false;
    private boolean muteOn = false;

    public void power() {
        powerOn = !powerOn;

        if (powerOn)
            System.out.println("power On");
        else
            System.out.println("power Off");
    }

    public void mute() {
        muteOn = !muteOn;

        if (muteOn)
            System.out.println("mute On");
        else
            System.out.println("mute Off");
    }
}
