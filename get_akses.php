<?php
// Konfigurasi database
require_once('koneksi.php');

// baca data dari request POST dalam format JSON
$json = file_get_contents('php://input');
$postData = json_decode($json, true);

$uid = $postData['uid'];

$sql = "SELECT uid FROM tb_mhs WHERE uid = :uid";
$stmt = $conn->prepare($sql);
$stmt->bindParam(':uid', $uid);
$stmt->execute();

$response = array();

if ($stmt->rowCount() > 0) {

    $cek  = "SELECT id, status FROM tb_akses WHERE uid = :uid ORDER BY tanggal DESC LIMIT 1";
    $stmt = $conn->prepare($cek);
    $stmt->bindParam(':uid', $uid);
    $stmt->execute();

    $last_status = $stmt->fetch(PDO::FETCH_ASSOC);
    $count = $stmt->rowCount();

    if ($last_status['status'] == 0 && $count > 0) {
        // simpan data di database
        $sql = "UPDATE tb_akses SET jam_keluar = CURTIME(), status = 1 WHERE id = :id";
        $stmt = $conn->prepare($sql);
        $stmt->bindParam(':id', $last_status['id']);
        $stmt->execute();

        $response['status'] = "success";
        $response['message'] = "Akses Keluar";
    } else {
        // simpan data di database
        $sql = "INSERT INTO tb_akses (uid, tanggal, jam_masuk, jam_keluar)
        VALUES (:uid, CURDATE(), CURTIME(), '')";
        $stmt = $conn->prepare($sql);
        $stmt->bindParam(':uid', $uid);
        $stmt->execute();

        $response['status'] = "success";
        $response['message'] = "Akses Diterima";
    }
} else {
    $response['status'] = "error";
    $response['message'] = "UID kartu tidak terdaftar";
}
header('Content-Type: application/json');
echo json_encode($response);
