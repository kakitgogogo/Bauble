import java.sql.*;

public class MySQLEx {
	static private Connection conn;
	static int cnt = 0;

	public static void main(String args[]) {
		if(!connect()) {
			System.out.println("Connection Error!");
			return;
		}
		ResultSet rs = executeQuery("select * from stu");
		showStuInfo(rs);
	}

	private static boolean connect() {
		String connectStr = "jdbc:mysql://localhost:3306/teaching?autoReconnect=true&useUnicode=true&characterEncoding=UTF-8&useSSL=false";
		try {
			Class.forName("com.mysql.jdbc.Driver");
			conn = DriverManager.getConnection(connectStr, "root", "131413");
			return true;
		}
		catch(Exception e) {
			System.out.println(e.getMessage());
			e.printStackTrace();
		}
		return false;
	}

	private static ResultSet executeQuery(String sqlSentence) {
		Statement stat;
		ResultSet rs = null;

		try {
			stat = conn.createStatement();
			rs = stat.executeQuery(sqlSentence);
		}
		catch(Exception e) {
			System.out.println(e.getMessage());
		}
		return rs;
	}

	private static void showStuInfo(ResultSet rs) {
		try {
			while(rs.next()) {
				System.out.println(rs.getString("name"));
			}
		}
		catch(Exception e) {
			System.out.println(e.getMessage());
		}
	}
}