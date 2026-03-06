#!/bin/bash

jails=(
        sshd
        dovecot
        postfix
        postfix-rbl
        postfix-sasl
)
step=8
delimiter="╬═════════════════╬══════════╬══════════╬\n"
time_cur=$(date +%s)

printf "$delimiter║ %-15s ║ %-8s ║ %-8s ║\n" "IP address" "for" "left"

for jail in "${jails[@]}";
do
        f2bstr=($(fail2ban-client get $jail banip --with-time))
        words_amount=${#f2bstr[@]}

        [ $words_amount -eq 0 ] && continue
        
        printf "$delimiter║ \e[1;32m%-16s\e[0m%-13s%-13s%1s\n" "$jail" "║" "║" "║" 
        last_index=$((words_amount-1))
        
        for ((index=0; index<=last_index; index+=step));
        do
                ipaddr=${f2bstr[$index+0]}
                time_ban=${f2bstr[$index+4]}
                time_end=$(date -d "${f2bstr[$index+6]} ${f2bstr[$index+7]}" +%s)
                time_dif=$((time_end-time_cur))
                printf -v ban_formatted "%02d:%02d:%02d" $((time_ban/3600)) $((time_ban%3600/60)) $((time_ban%60))
                printf -v dif_formatted "%02d:%02d:%02d" $((time_dif/3600)) $((time_dif%3600/60)) $((time_dif%60))
                printf "║ %-15s ║ %-8s ║ %-8s ║\n" "$ipaddr" "$ban_formatted" "$dif_formatted"
        
        done
done

printf "$delimiter"
