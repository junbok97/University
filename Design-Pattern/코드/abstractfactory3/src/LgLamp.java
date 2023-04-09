public class LgLamp extends DirectionLamp {
    @Override
    protected void doLight(Direction direction) {
        System.out.println("Lg Lamp "+direction);
    }
}
