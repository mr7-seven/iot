$uid = "KODE_UID_KARTU"; // UID kartu yang ingin dijadikan acuan

// Mengambil saldo saat ini dari database

$sql = "SELECT saldo FROM pengguna WHERE uid = :uid";

$stmt = $pdo->prepare($sql);

$stmt->bindParam(':uid', $uid);

$stmt->execute();

$response = array();

if ($stmt->rowCount() > 0) {

    $saldo = $stmt->fetchColumn();

    if ($saldo >= 5000) {

        // Lakukan pengurangan saldo

        $newSaldo = $saldo - 5000;

        // Memperbarui saldo di database

        $sql = "UPDATE pengguna SET saldo = :newSaldo WHERE uid = :uid";

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

echo json_encode($response);

