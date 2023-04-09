public class Elevator {
    private Motor motor;
    private Door door;
    private DirectionLamp lamp;

    public void setLamp(DirectionLamp lamp) {
        this.lamp = lamp;
    }
    public void setMotor(Motor motor) {
        this.motor = motor;
    }

    public void setDoor(Door door) {
        this.door = door;
    }

    public void move(Direction direction) {
        motor.move(direction);
        lamp.doLight(direction);
    }
}
