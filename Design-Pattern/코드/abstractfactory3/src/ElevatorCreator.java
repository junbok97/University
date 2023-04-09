public class ElevatorCreator {
    public static Elevator assembleElevator(ElevatorAbstractFactory factory) {
        Elevator elevator = factory.createElevator();
        Motor motor = factory.createMotor();
        elevator.setMotor(motor);
        Door door = factory.createDoor();
        elevator.setDoor(door);
        motor.setDoor(door);
        DirectionLamp lamp = factory.createLamp();
        elevator.setLamp(lamp);
        return elevator;
    }
    public static void main(String[] args) {
        ElevatorAbstractFactory factory = null;
        if (args[0].equalsIgnoreCase("LG")) factory = new LgElevatorFactory();
        else if (args[0].equalsIgnoreCase("Hyundai")) factory = new HyundaiElevatorFactory();
        else factory = new SamsungElevatorFactory();
        Elevator elevator = assembleElevator(factory);
        elevator.move(Direction.UP);
    }
}
