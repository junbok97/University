public class Controller {
    private Command command;

    public void setCommand(Command command) {
        this.command = command;
    }

    public void exec() {
        command.exec();
    }
}
