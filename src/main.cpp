/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: glamazer <glamazer@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 11:33:15 by glamazer          #+#    #+#             */
/*   Updated: 2024/03/25 10:11:23 by glamazer         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Application.hpp"

int main(int argc, char **argv)
{
    if (argc < 2)
    {
        cerr << "Usage: ./scop <obj_file> <texture.bmp>(optional)" << endl;
        return -1;
    }
    if (argc == 3)
    {
        Application app(argv[1], argv[2]);
        app.run();
        return 0;
    } else {
        Application app(argv[1], "");
        app.run();
        return 0;
    }
    return 0;
}