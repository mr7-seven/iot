app.get("/transactions", (req, res) => {
  // Retrieve transactions from database
  db.all(
    "SELECT * FROM user_uid JOIN transaksi ON user_uid.id = transaksi.id_user",
    (err, rows) => {
      if (err) {
        console.error("Error fetching transactions:", err);
        res.status(500).send("Internal Server Error");
      } else {
        res.json(rows);
        console.log(rows);
      }
    }
  );
});

// Handle incoming messages
client.on('message', function (topic, message) {
    if (topic === "rfid/request") {
      try {
        const data = JSON.parse(message.toString());
        const uid = data.uid;
        console.log("RFID Tag ID:", uid);
        // Check if UID exists in user_uid table
        db.all(
          "SELECT id, saldo FROM user_uid WHERE uid = ?",
          [uid],
          (err, rows) => {
            if (err) {
              console.error("Error querying database:", err);
              return;
            }
            if (rows.length > 0) {
              // UID exists, perform balance deduction
              console.log("UID exists in database");

              // Get user ID and saldo from the result
              const userId = rows[0].id;
              const saldo = rows[0].saldo;

              // Check if saldo is sufficient for deduction
              if (saldo >= 25000) {
                // Deduct saldo by 25000
                const newSaldo = saldo - 25000;

                // Record the transaction
                const currentTime = new Date().toISOString();
                db.run(
                  "INSERT INTO transaksi (id_user, jumlah_pembayaran, waktu_transaksi) VALUES (?, ?, ?)",
                  [userId, 25000, currentTime],
                  function (err) {
                    if (err) {
                      console.error("Error recording transaction:", err);
                      return;
                    }
                    console.log("Transaction recorded successfully");

                    // Update user's saldo
                    db.run(
                      "UPDATE user_uid SET saldo = ? WHERE id = ?",
                      [newSaldo, userId],
                      function (err) {
                        if (err) {
                          console.error("Error updating saldo:", err);
                          return;
                        }
                        console.log("User's saldo updated successfully");

                        // Send success response
                        client.publish("rfid/response", "Success");
                      }
                    );
                  }
                );
              } else {
                // Insufficient saldo
                console.log("Insufficient saldo for deduction");
                // Send failure response
                client.publish("rfid/response", "Insufficient saldo");
              }
            } else {
              // UID does not exist
              console.log("UID does not exist in database");
              // Send failure response
              client.publish("rfid/response", "UID not registered");
            }
          }
        );
      } catch (error) {
        console.error("Error parsing JSON:", error);
      }
    }
});
