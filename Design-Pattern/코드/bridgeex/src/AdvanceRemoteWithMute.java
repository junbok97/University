public class AdvanceRemoteWithMute extends BasicRemote {
    public AdvanceRemoteWithMute(Device device) {
        super(device);
    }
    public void mute() {
        System.out.println("Remote: mute");
        device.setVolume(0);
    }
}
