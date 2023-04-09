public abstract class Motor {
    private Door door;
    private MotorStatus motorStatus;
    public void setDoor(Door door) {
        this.door = door;
        this.motorStatus = MotorStatus.STOPPED;
    }

    public void move(Direction direction) {
        MotorStatus motorStatus = getMotorStatus();
        if (motorStatus == MotorStatus.MOVING) return;
        DoorStatus doorStatus = door.getDoorStatus();
        if (doorStatus == DoorStatus.OPENED) door.close();
        moveMotor(direction);
        setMotorStatus(MotorStatus.MOVING);
    }

    private void setMotorStatus(MotorStatus motorStatus) {
        this.motorStatus = motorStatus;
    }

    protected abstract void moveMotor(Direction direction);

    private MotorStatus getMotorStatus() {
        return motorStatus;
    }
}
