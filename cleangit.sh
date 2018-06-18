# start "C:\Program Files\Git\git-bash.exe"

git gc --prune=now
hash=$(git verify-pack -v .git/objects/pack/*.idx | sort -k 3 -n | tail -5 | cut -f1 -d" ")

for i in $hash;  
do   
filename=$(git rev-list --objects --all | grep $i | cut -f1 --complement -d" ")
echo $filename
git filter-branch --force --index-filter "git rm --cached --ignore-unmatch ""$filename"
done

rm -rf .git/refs/original/
git reflog expire --expire=now --all
git fsck --full --unreachable
git repack -A -d
git gc --aggressive --prune=now

# 