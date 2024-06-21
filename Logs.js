function login() {
  const username = document.getElementById('username').value;
  const password = document.getElementById('password').value;

  // Here you can add your authentication logic, for simplicity using a sample username and password
  if (username === '1234' && password === '1234') {
    document.getElementById('loginPage').style.display = 'none';
    document.getElementById('logsPage').style.display = 'block';
    displayLogs();
  } else {
    alert('Invalid credentials. Please try again.');
  }
}

function displayLogs() {
  fetch('log.php')
  .then(response => response.json())
  .then(data => {
    const logsList = document.getElementById('logsList');
    logsList.innerHTML = ''; // پاک کردن لاگ‌های قبلی

    data.forEach(log => {
      const li = document.createElement('li');
      li.textContent = log;
      logsList.appendChild(li);
    });
  })
  .catch(error => console.error('Error:', error));
}
