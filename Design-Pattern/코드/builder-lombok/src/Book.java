import lombok.Builder;
import lombok.Getter;
import lombok.NonNull;
import lombok.Setter;

@Getter
@Builder
public class Book {
    @NonNull
    private Long id; //필수
    @NonNull
    private String isbn; //필수
    private String title;
    private String author;
    private int pages;
    private String category;


}
