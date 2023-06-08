<?php
// Konfigurasi database
require_once('db_config.php');

// Ambil data dari database
$sql = "SELECT * FROM tb_data ORDER BY waktu DESC";
$result = $conn->query($sql);

// Buat array untuk menampung data
$data = array();

// Loop untuk menambahkan data ke dalam array
if ($result->rowCount() > 0) {
    while($row = $result->fetch(PDO::FETCH_ASSOC)) {
        $data[] = $row;
    }
}

// Kembalikan data dalam bentuk JSON
header('Content-Type: application/json');
echo json_encode($data);

// Tutup koneksi
$db = null;
?>
