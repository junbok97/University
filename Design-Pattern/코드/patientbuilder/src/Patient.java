import lombok.Builder;
import lombok.Getter;

@Builder
@Getter
public class Patient {
    private Long id;
    private String name;
    private BloodType bType ;
    private float height;
    private float weight;
    private PhoneNo contact;

}