<?php
include 'koneksi.php';

// Ambil nilai UID, nama, dan alamat dari POST data
$uid = $_POST['uid'];
$nama = $_POST['nama'];
$alamat = $_POST['alamat'];

// Query INSERT untuk menyimpan data karyawan ke tabel tb_karyawan
$stmt = $conn->prepare("INSERT INTO tb_karyawan (uid, nama, alamat) VALUES (:uid, :nama, :alamat)");
$stmt->bindParam(':uid', $uid);
$stmt->bindParam(':nama', $nama);
$stmt->bindParam(':alamat', $alamat);
$stmt->execute();

echo "Data karyawan berhasil disimpan";

$conn = null;
