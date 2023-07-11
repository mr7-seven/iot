<?php
include 'koneksi.php';

// Query SELECT untuk mengambil UID terakhir dari tabel rfid_data
$stmt = $conn->query("SELECT uid FROM tb_uid ORDER BY id DESC LIMIT 1");
$result = $stmt->fetch(PDO::FETCH_ASSOC);

if ($result) {
    $uid = $result['uid'];
    $response = array('uid' => $uid);
    header('Content-Type: application/json');
    echo json_encode($response);
} else {
    $response = array('error' => 'No UID found');
    header('Content-Type: application/json');
    echo json_encode($response);
}

$conn = null;
