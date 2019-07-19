# Chapter 9 Process Credentials

每個 Process 有一系列的UID/GID，這些東西又叫 Process Credentials

1. Real user ID / Real group ID
2. Effective user ID / Effective group ID
3. Saved set-user-ID / Saved set-group-ID
4. File-system user ID / File-system group ID
5. 輔助ID

Real user/group ID 在使用者登入時，Shell 檢查 /etc/passwd 檔案取得使用者的ID並設定為該用戶的實際 Real user/group ID，後續該Shell建立的Process都會繼承這個 Real user/group ID

Effective user/group ID, kernel 根據Process的值給予相應的權限和判定某些操作的有效性。 這個值可以藉由某些 system call 或是 set-user-id set-group-id 來變換。 當 Effective user ID 為 0 時，該 Process 擁有最高權限。

Saved set-user/group ID ，當 Process 執行，這個值會被設為目前的Effective user/group ID，如果執行檔有啟用 set user id，這個值就是執行檔的擁有者ID，主要用途是當作swap的中繼站，你可以透過某些system call來使Effective ID變成原始的執行者Real ID，這樣的好處是可以適時地減少程式的權限，只在必要時切換成別的ID，而這個Saved set ID的用途就是在當中繼站，可以在切換時，把原先的Set user ID擺過來。

File-system ID 決定 Process 針對 開啟檔案/改變檔案屬性/修改文件權限 等操作的能力，基本上現在這個ID絕大多數時間同於Effective ID。如果Effective ID被改動了，File-system ID也會隨之變化，除非使用某些 system call 這個ID才有可能變得不大一樣
在過去，修改文件的操作由 Effective ID 決定，某些文件系統要替某使用者戳文件時，要把自己的Effective ID改得跟那位使用者一樣，但這樣可能會遇到信號轟炸等等壞壞情況，為此新增了這個File-system ID，linux kernel 從 2.0 起，遵循SUSv3的規則，現在已經沒有這個問題了，這個ID的存在意義只剩向後間容。
似乎只有linux系統有這個ID？

輔助ID表示 Process 所屬的組別。從某版本開始一個使用者可以屬於多個group，這個輔助ID就是在紀錄那額外的group ID們。

綜合上述的ID，可以決定出Process訪問各種資源的權限。

訪問/proc/PID/status可以取得ID訊息
