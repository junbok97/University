public class BirdFlyCmd implements Command {
    private Bird bird;

    public BirdFlyCmd(Bird bird) {
        this.bird = bird;
    }

    @Override
    public void exec() {
        bird.fly();
    }
}
