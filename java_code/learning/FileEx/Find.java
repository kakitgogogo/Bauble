import java.io.File;
import java.util.*;

public class Find {
	public static void find(String dir, String filename) {
		File root = new File(dir);
		for(File file:root.listFiles()) {
			if(file.isDirectory()) {
				find(file.getAbsolutePath(), filename);
			}
			else {
				if(file.getName().equals(filename))
				{
					System.out.println(file.getAbsolutePath());
				}
			}
		}
	}

	public static void main(String[] args) {
		if(args.length != 2) {
			System.out.format("Usage: Find dir filename");
		}
		find(args[0], args[1]);
	}
}