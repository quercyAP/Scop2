#!/bin/bash

projectname="webserv"
srcdir="./src/"
includesdir="./includes/"





dirname=$1
filename=$2
len=${#filename}
if [ ! -d "$srcdir$dirname" ]; then
  mkdir -p $srcdir$dirname
fi
if [ ! -d "$includesdir$dirname" ]; then
  mkdir -p $includesdir$dirname
fi




# Fait le .cpp
if [ -e "$srcdir$dirname/$filename.cpp" ]; then
  echo "Le fichier $filename.cpp existe déjà"
else
    echo -n "/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   " >> "$srcdir$dirname/$filename.cpp"
echo -n "$filename.cpp" >> "$srcdir$dirname/$filename.cpp"
i=0
while [ $i -le $(expr 46 - $len) ]
do
echo -n ' ' >> "$srcdir$dirname/$filename.cpp"
i=$((i+1))
done
echo ":+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
" >> "$srcdir$dirname/$filename.cpp"
  echo "#include \"$filename.hpp\"
" >> "$srcdir$dirname/$filename.cpp"
  echo "Le fichier $filename.cpp a été créé avec succès"
fi





# Fait le .hpp
if [ -e "$includesdir$dirname/$filename.hpp" ]; then
  echo "Le fichier $filename.hpp existe déjà"
else
  echo -n "/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   " >> "$includesdir$dirname/$filename.hpp"
echo -n "$filename.hpp" >> "$includesdir$dirname/$filename.hpp"
i=0
while [ $i -le $(expr 46 - $len) ]
do
echo -n ' ' >> "$includesdir$dirname/$filename.hpp"
i=$((i+1))
done
echo ":+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42mulhouse.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/01/26 08:45:59 by glamazer          #+#    #+#             */
/*   Updated: 2023/03/23 09:40:43 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */
" >> "$includesdir$dirname/$filename.hpp"
echo "#ifndef $(echo "$filename" | tr '[:lower:]' '[:upper:]')_HPP" >> "$includesdir$dirname/$filename.hpp"
echo "# define $(echo "$filename" | tr '[:lower:]' '[:upper:]')_HPP
" >> "$includesdir$dirname/$filename.hpp"
echo "#endif" >> "$includesdir$dirname/$filename.hpp"
echo "Le fichier $filename.hpp a été créé avec succès"
fi