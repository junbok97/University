public class Monkey {
    private MovingStratgey movingStratgey;

    public Monkey(MovingStratgey movingStratgey) {
        this.movingStratgey = movingStratgey;
    }

    public void setMovingStratgey(MovingStratgey movingStratgey) {
        this.movingStratgey = movingStratgey;
    }

    public void exec() {
        movingStratgey.move();
    }
}
