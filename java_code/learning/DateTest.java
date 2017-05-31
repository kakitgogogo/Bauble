import java.util.*;
import java.text.*;

public class DateTest {
	public static void main(String args[])
	{
		Date now = new Date();
		System.out.println(now);

		Calendar cal = Calendar.getInstance();
		cal.setTime(new Date());
		System.out.println(cal.get(Calendar.DAY_OF_MONTH));
		System.out.println(cal.get(Calendar.DAY_OF_WEEK));
		System.out.println(cal.get(Calendar.HOUR));
	}
}