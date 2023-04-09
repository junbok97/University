public class Main {
    public static void main(String[] args) {
        Patient p = new Patient.PatientBuilder().id(1L).name("insang").build();
        String name = p.getName();
        System.out.println(name);
    }
}
