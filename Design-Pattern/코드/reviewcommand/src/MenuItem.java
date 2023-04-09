public class MenuItem {
    private Command cmd;

    public void setCmd(Command cmd) {
        this.cmd = cmd;
    }

    public void buttonPressed() {
      cmd.execute();
    }
}
