<?php
// Koneksi ke database
$servername = "localhost";
$username = "username";
$password = "password";
$dbname = "database_name";

try {
	$conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
	// Atur mode error PDO ke exception
	$conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
}
catch(PDOException $e) {
	echo "Koneksi gagal: " . $e->getMessage();
}

// Terima input dari form login
$username = $_POST['username'];
$password = md5($_POST['password']);

// Periksa apakah username dan password ada di database
$stmt = $conn->prepare("SELECT * FROM users WHERE username = :username AND password = :password");
$stmt->bindParam(':username', $username);
$stmt->bindParam(':password', $password);
$stmt->execute();
$result = $stmt->fetch(PDO::FETCH_ASSOC);

if($result) {
	echo "Login sukses!";
}
else {
	echo "Login gagal!";
}
?>
