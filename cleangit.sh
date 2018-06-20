# start "C:\Program Files\Git\git-bash.exe"

git gc --prune=now
hash=$(git verify-pack -v .git/objects/pack/*.idx | sort -k 3 -n | tail -1 | cut -f1 -d" ")

for i in $hash;  
do   
filename=$(git rev-list --objects --all | grep $i | cut -f1 --complement -d" ")
echo $filename
git filter-branch --force --index-filter "git rm --cached --ignore-unmatch ""$filename"   --prune-empty --tag-name-filter cat -- --all
done

git for-each-ref --format='delete %(refname)' refs/original | git update-ref --stdin
git reflog expire --expire=now --all
git gc --prune=now