    // Function to fetch and update transaction data
    function fetchAndUpdateData() {
        $.getJSON('/transactions', function(transactions) {
            // Clear existing table rows
            $('#transactionTable tbody').empty();
            console.log(transactions[0].nama);

            // Append new rows with updated data
            $.each(transactions, function(index, transaction) {
                $('#transactionTable tbody').append(`<tr><td>${transaction.id}</td><td>${transaction.uid}</td>
                                                    <td>${transaction.nama}</td><td>${transaction.saldo}</td>
                                                    <td>${transaction.id_user}</td><td>${transaction.jumlah_pembayaran}</td>
                                                    <td>${transaction.waktu_transaksi}</td></tr>`);
            });
        })
        .fail(function(error) {
            console.error('Error fetching transactions:', error);
        });
    }

    // Initial fetch and update
    fetchAndUpdateData();

    // Set interval to fetch and update data every 30 seconds
    setInterval(fetchAndUpdateData, 30000);
