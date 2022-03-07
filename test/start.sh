echo "Username:"
read uservar
echo $uservar > src/userinfo
python3 src/app.py
