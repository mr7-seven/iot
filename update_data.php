<?php
// Koneksi ke database
include 'db_config.php';

try {
    // Ambil nilai input dari permintaan AJAX
    $uid = $_POST['uid'];
    $saldo = $_POST['saldo'];

    // Lakukan query untuk update data
    $query = "UPDATE tb_data SET saldo = :saldo WHERE uid = :uid";
    $stmt = $db->prepare($query);
    $stmt->bindParam(':saldo', $saldo);
    $stmt->bindParam(':uid', $uid);
    // Ganti ':id' dengan nilai ID yang sesuai

    // Eksekusi query
    if ($stmt->execute()) {
        $response = "Data berhasil diupdate";
        echo $response;
    } else {
        $response = "Gagal meng-update data";
        echo $response;
    }
} catch (PDOException $e) {
    $response = "Error: " . $e->getMessage();
    echo $response;
}
