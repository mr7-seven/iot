<?php
include "koneksi.php";

// Eksekusi query
$sql = "SELECT * FROM tb_karyawan
";


$stmt = $conn->prepare($sql);
$stmt->execute();

// Mendapatkan data dari query
$data = array();
while ($row = $stmt->fetch(PDO::FETCH_ASSOC)) {
    $data[] = $row;
}
// Menyusun struktur data untuk respons DataTables
$response = array(
    'draw' => 1,
    'recordsTotal' => count($data),
    'recordsFiltered' => count($data),
    'data' => $data
);

// Mengirimkan respons JSON
header('Content-Type: application/json');
echo json_encode($response);
