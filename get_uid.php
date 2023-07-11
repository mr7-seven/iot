<?php

$servername = "localhost";
$username = "root";
$password = "";
$dbname = "db_presensi";

try {
  $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
  $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

  $uid = $_GET['uid']; // Ganti dengan nilai UID yang ingin diperbarui

  // Query UPDATE untuk memperbarui UID dalam tabel tb_uid
  $stmt = $conn->prepare("UPDATE tb_uid SET uid = :uid ORDER BY id DESC LIMIT 1");
  $stmt->bindParam(':uid', $uid);
  $stmt->execute();

  echo "UID updated successfully";
} catch(PDOException $e) {
  echo "Error: " . $e->getMessage();
}

$conn = null;
