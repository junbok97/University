public class Main {
    public static void main(String[] args) {
        MenuItem m = new MenuItem();
        Bird bird = new Bird();
        Command cmd1 = new BirdSingCmd(bird);
        m.setCmd(cmd1);
        m.buttonPressed();
        Tv tv = new Tv();
        Command cmd2 = new TvPwrCmd(tv);
        m.setCmd(cmd2);
        m.buttonPressed();
        FileNew f = new FileNew();
        Command cmd3 = new FileNewCmd(f);
        m.setCmd(cmd3);
        m.buttonPressed();
    }
}
