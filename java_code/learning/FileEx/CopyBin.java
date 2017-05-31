import  java.io.*;

public class CopyBin {
	public static void copyBinaryFile(String src, String dst) throws Exception {
		InputStream in = new FileInputStream(src);
		OutputStream out = new FileOutputStream(dst);
		byte[] bytes = new byte[100];
		int byteread = 0;

		while((byteread = in.read(bytes)) != -1) {
			out.write(bytes, 0, byteread);
		}	
		in.close();
		out.close();
	}

	public static void main(String[] args) {
		String src = args[0];
		String dst = args[1];
		try {
			copyBinaryFile(src, dst);
		}
		catch(Exception e) {
			System.out.println(e);
		}
		
	}
}