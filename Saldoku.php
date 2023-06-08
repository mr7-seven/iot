<?php
// Konfigurasi database
require_once('db_config.php');

// baca data dari request POST dalam format JSON
$json = file_get_contents('php://input');
$postData = json_decode($json, true);

$uid = $postData['uid'];

$sql = "SELECT saldo FROM tb_data WHERE uid = :uid";
$stmt = $pdo->prepare($sql);
$stmt->bindParam(':uid', $uid);
$stmt->execute();

$response = array();

if ($stmt->rowCount() > 0) {
    $saldo = $stmt->fetchColumn();
    if ($saldo >= 50000) {

        // Lakukan pengurangan saldo
        $newSaldo = $saldo - 50000;

        // Memperbarui saldo di database
        $sql = "UPDATE tb_data SET saldo = :newSaldo WHERE uid = :uid";
        $stmt = $pdo->prepare($sql);
        $stmt->bindParam(':newSaldo', $newSaldo);
        $stmt->bindParam(':uid', $uid);
        $stmt->execute();

        $response['status'] = "success";
        $response['message'] = "Saldo berhasil dikurangi";
        $response['saldo'] = $newSaldo;

    } else {
        $response['status'] = "error";
        $response['message'] = "Saldo Anda tidak cukup";
        $response['saldo'] = $saldo;
    }

} else {
    $response['status'] = "error";
    $response['message'] = "UID kartu tidak ditemukan";
}
header('Content-Type: application/json');
echo json_encode($response);
?>
